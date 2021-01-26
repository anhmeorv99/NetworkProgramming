import django_filters
import requests
from django_filters import rest_framework as filters
from rest_framework import viewsets, status
from rest_framework.permissions import AllowAny
from rest_framework import filters as rest_filters
from django_filters import rest_framework as filters
from rest_framework.response import Response

from App import settings
from chat_app_service.serializer.friends import FriendsSerializer
from chat_app_service.serializer.users import UserSerializer
from chat_app_service.models import Friends


class FriendsFilter(filters.FilterSet):
    confirm = filters.BooleanFilter()
    user = filters.NumberFilter()
    friend = filters.NumberFilter()

    class Meta:
        model = Friends
        fields = ['confirm', 'user', 'friend']


class FriendsViewSet(viewsets.ModelViewSet):
    queryset = Friends.objects.all()
    permission_classes = (AllowAny,)
    serializer_class = FriendsSerializer
    filter_backends = (filters.DjangoFilterBackend, rest_filters.SearchFilter, rest_filters.OrderingFilter)

    filter_class = FriendsFilter

    def list(self, request, *args, **kwargs):
        queryset = self.filter_queryset(self.get_queryset())
        page = self.paginate_queryset(queryset)
        if page is not None:
            serializer = self.get_serializer(page, many=True)
            return self.get_paginated_response(serializer.data)

        serializer = self.get_serializer(queryset, many=True)
        return Response(serializer.data)

    def create(self, request, *args, **kwargs):
        serializer = self.get_serializer(data=request.data)
        serializer.is_valid(raise_exception=True)
        friend = request.data.get('friend', None)
        if friend:
            req = requests.get(f'http://{settings.ALLOWED_HOSTS[0]}:8000/api/user/{str(friend)}/')
            if req.status_code != 404:
                self.perform_create(serializer)
                headers = self.get_success_headers(serializer.data)
                return Response(serializer.data, status=status.HTTP_201_CREATED, headers=headers)
            else:
                return Response(data={"NOT FOUND FRIEND"}, status=status.HTTP_404_NOT_FOUND)
        else:
            return Response(data={"FRIEND NOT NULL"}, status=status.HTTP_400_BAD_REQUEST)
        
    def destroy(self, request, *args, **kwargs):
        instance = self.get_object()
        self.perform_destroy(instance)
        return Response(data={"success": "Deleted !"}, status=status.HTTP_204_NO_CONTENT)
