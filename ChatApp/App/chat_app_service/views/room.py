from django_filters import rest_framework as filters
from rest_framework import viewsets
from rest_framework.permissions import AllowAny
from rest_framework import filters as rest_filters
from rest_framework.response import Response

from chat_app_service.serializer.room import RoomSerializer
from chat_app_service.models import Room


class RoomViewSet(viewsets.ModelViewSet):
    queryset = Room.objects.all()
    permission_classes = (AllowAny,)
    serializer_class = RoomSerializer
    filter_backends = (filters.DjangoFilterBackend, rest_filters.SearchFilter, rest_filters.OrderingFilter)

    def list(self, request, *args, **kwargs):
        queryset = self.filter_queryset(self.get_queryset())
        user = request.query_params.get('user')
        if user:
            queryset = Room.objects.filter(member=user)

        page = self.paginate_queryset(queryset)
        if page is not None:
            serializer = self.get_serializer(page, many=True)
            return self.get_paginated_response(serializer.data)

        serializer = self.get_serializer(queryset, many=True)
        return Response(serializer.data)
