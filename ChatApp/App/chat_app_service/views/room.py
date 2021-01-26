from django_filters import rest_framework as filters
from rest_framework import viewsets, status
from rest_framework.permissions import AllowAny
from rest_framework import filters as rest_filters
from rest_framework.response import Response

from App import settings
from chat_app_service.serializer.room import RoomSerializer
from chat_app_service.models import Room
import requests


class RoomViewSet(viewsets.ModelViewSet):
    queryset = Room.objects.all()
    permission_classes = (AllowAny,)
    serializer_class = RoomSerializer
    filter_backends = (filters.DjangoFilterBackend, rest_filters.SearchFilter, rest_filters.OrderingFilter)

    def list(self, request, *args, **kwargs):
        queryset = self.filter_queryset(self.get_queryset())
        user = request.query_params.get('user')
        if user is not None:
            queryset = Room.objects.filter(member=user)
        page = self.paginate_queryset(queryset)
        if page is not None:
            serializer = self.get_serializer(page, many=True)
            return self.get_paginated_response(serializer.data)

        serializer = self.get_serializer(queryset, many=True)
        return Response(serializer.data)

    def update(self, request, *args, **kwargs):
        partial = kwargs.pop('partial', False)
        instance = self.get_object()
        member = request.query_params.get('add_member')
        room = request.query_params.get('room')
        if member is not None and room is not None:
            req = requests.get(f"http://{settings.ALLOWED_HOSTS[0]}:8000/api/room/{room}/")
            list_member = req.json()['member']
            list_member.append(int(member))
            data = {"member": list_member}

            serializer = self.get_serializer(instance, data=data, partial=partial)
        else:
            serializer = self.get_serializer(instance, data=request.data, partial=partial)
        serializer.is_valid(raise_exception=True)
        self.perform_update(serializer)

        if getattr(instance, '_prefetched_objects_cache', None):
            # If 'prefetch_related' has been applied to a queryset, we need to
            # forcibly invalidate the prefetch cache on the instance.
            instance._prefetched_objects_cache = {}

        return Response(serializer.data)
